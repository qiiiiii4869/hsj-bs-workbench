"""Documentation coverage and examples, independent of business calculations."""
import unittest
from copy import deepcopy
import api
from api_contracts import OPERATIONS,SCHEMAS,SPECIAL_EXAMPLES

try:
    from jsonschema import Draft202012Validator
    from openapi_spec_validator import validate
except ImportError:
    Draft202012Validator=None


class ApiDocsTests(unittest.TestCase):
    def setUp(self):
        api.app.openapi_schema=None
        self.schema=api.document_schema(api.app)

    def test_every_runtime_api_is_documented(self):
        actual={(r.path,m.lower()) for r in api.app.routes if getattr(r,'path','').startswith('/api/') for m in getattr(r,'methods',[]) if m in ('GET','POST')}
        documented={(p,m) for p,methods in self.schema['paths'].items() for m in methods if m in ('get','post')}
        self.assertEqual(actual,documented)
        self.assertEqual(len(documented),34)

    def test_every_success_and_error_response_has_structure(self):
        for path,contract in OPERATIONS.items():
            operation=self.schema['paths'][path][contract['method']]
            self.assertTrue(operation['description'],path)
            for code,response in operation['responses'].items():
                self.assertTrue(response['content'],(path,code))
                for media in response['content'].values():self.assertTrue(media['schema'],(path,code))
            if contract['method']=='post':
                self.assertEqual(operation['security'],[{'HSJSession':[]}])
                self.assertTrue(operation['requestBody']['content'],path)

    def test_special_modules_and_chart_actions_are_complete(self):
        self.assertEqual(set(SPECIAL_EXAMPLES),{'orbit','correlation','trend','balance','balance_sample','balance_preview','balance_decompose','jigger','efficiency','cam','airgap'})
        chart=self.schema['paths']['/api/chart']['post']['requestBody']['content']['application/json']
        self.assertEqual(set(chart['examples']),{'nearest','nearest_xy','detail','extrema','index'})
        for key in ('y','y_min','y_max','width','height'):self.assertIn(key,SCHEMAS['ChartRequest']['properties'])

    @unittest.skipUnless(Draft202012Validator,'可选文档校验依赖未安装')
    def test_openapi_and_all_examples_validate(self):
        validate(self.schema)
        for path,contract in OPERATIONS.items():
            request=contract['request']
            if request is None:continue
            validator=Draft202012Validator({**deepcopy(request),'components':self.schema['components']})
            for name,example in contract['examples'].items():
                with self.subTest(path=path,example=name):validator.validate(example['value'])


if __name__=='__main__':unittest.main(verbosity=2)
